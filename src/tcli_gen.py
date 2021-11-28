import sys
import os
from termios import CEOL

class Cmd:
	def __init__(self):
		self.keywords = []
		self.args = []
		self.structName = ''

class Arg:
	def __init__(self):
		self.required = False
		self.type = ''
		self.shortName = ''
		self.longName = ''
		self.optIdx = 0
		self.mutexIdx = 0
		self.cVarName = ''

def formatLine(line):
	line = line.strip()
	line = line.replace('(', ' ( ')
	line = line.replace(')', ' ) ')
	line = line.replace('[', ' [ ')
	line = line.replace(']', ' ] ')
	line = line.replace('|', ' | ')
	return line

def formatCVarName(name):
	while name.startswith('-'):
		name = name[1:]
	cVarName = ''
	for c in name:
		cVarName += c if c.isalnum() else '_'
	return cVarName

def hashCalc(buf):
	hash = 0x811c9dc5
	for c in buf:
		hash ^= ord(c)
		hash *= 0x01000193
		hash &= 0xffffffff
	return hash

def main():
	debug = False
	cmds = []
	EOL = '\n'

	with open('tcli_def.txt', 'r') as f:
		lines = f.readlines()

	for line in lines:
		line = formatLine(line)
		if debug:
			print('Parsing line: "' + line + '"')
		cmd = Cmd()
		parsingKeywords = True
		required = 1
		optBoolIdx = 0
		optValIdx = 0
		mutex = 1
		for word in line.split():
			# print('Parsing word ' + word)
			if parsingKeywords:
				if word[0].isalpha():
					cmd.keywords.append(word)
					cmd.structName += '_' + word
				else:
					parsingKeywords = False
			if not parsingKeywords:
				if word == '(':
					required = 1
				elif word == ')':
					required = 1
				elif word == '[':
					required = 0
				elif word == ']':
					required = 1
				elif word == '|':
					mutex = 0
				else:
					arg = Arg()
					if word.startswith('<'):
						if word.endswith('>...'):
							arg.type = 'posMulti'
							word = word[1:-4]
						elif word.endswith('>'):
							arg.type = 'pos'
							word = word[1:-1]
						else:
							print('Unrecognized "' + word + '"')
							return -1
						arg.cVarName = formatCVarName(word)
						arg.longName = word
						arg.optIdx = optValIdx
						optValIdx += 1
					else:
						opt, sep, oprnd = word.partition('=')
						n1, sep2, n2 = opt.partition(',')
						if n2 != '':
							arg.shortName = n1
							arg.longName = n2
						else:
							arg.shortName = ''
							arg.longName = n1
						if sep == '=':
							arg.type = 'optVal'
							arg.optIdx = optValIdx
							optValIdx += 1
						else:
							arg.type = 'optBool'
							arg.optIdx = optBoolIdx
							optBoolIdx += 1
						arg.cVarName = formatCVarName(arg.longName)
					arg.mutex = mutex
					mutex = 1
					arg.required = required
					cmd.args.append(arg)
		if debug:
			print('Command: ', cmd.keywords)
			for arg in cmd.args:
				print('  Arg: shortName = "' + arg.shortName + '", longName = "' + arg.longName + '", cVarName = "' + arg.cVarName + '", type = ' + arg.type + ', required = ' + str(arg.required) + ', optIdx = ' + str(arg.optIdx) + ', mutex = ' + str(arg.mutex))

		cmds.append(cmd)
	cmdStringTbl = '*:'
	argStringTbl = ''
	for cmd in cmds:
		for keyword in cmd.keywords:
			if not keyword in cmdStringTbl:
				cmdStringTbl += keyword + ':'
		for arg in cmd.args:
			if not arg.longName in argStringTbl:
				argStringTbl += arg.longName + ':'
	cmdStringTbl += ':'
	argStringTbl += ':'
	if debug:
		print('CmdStringTbl: "' + cmdStringTbl + '"')
		print('ArgStringTbl: "' + argStringTbl + '"')

	x  = '#ifndef TCLI_DEF_H' + EOL
	x += '#define TCLI_DEF_H' + EOL
	x += '' + EOL
	x +=	'#include <stdint.h>' + EOL
	x +=	'' + EOL
	x +=	'enum' + EOL
	x +=	'{' + EOL
	x +=	'    CMD_ID_NONE = 0,' + EOL
	for cmd in cmds:
		x += '    CMD_ID' + cmd.structName + ',' + EOL
	x += '    CMD_ID_CNT' + EOL
	x += '};' + EOL
	x += '' + EOL
	x += 'typedef uint32_t bool_options_t;' + EOL
	x += '' + EOL
	maxVarArgCnt = 0
	for cmd in cmds:
		varArgCnt = 0
		x += 'typedef struct' + EOL
		x += '{' + EOL
		boolCnt = 0
		for arg in cmd.args:
			if arg.type == 'optBool':
				x += '    bool_options_t ' + arg.cVarName + ':1;' + EOL
				boolCnt += 1
		x += '    bool_options_t _pad_:' + str(32-boolCnt) + ';' + EOL		
		for arg in cmd.args:
			if arg.type != 'optBool':
				varArgCnt += 1
				x += '    char *' + arg.cVarName + ';' + EOL
		x += '} tcli_args' + cmd.structName + '_t;' + EOL
		x += '' + EOL
		if varArgCnt > maxVarArgCnt:
			maxVarArgCnt = varArgCnt
	x += 'typedef struct' + EOL
	x += '{' + EOL
	x += '    bool_options_t bools;' + EOL
	x += '    char *vals[' + str(maxVarArgCnt) + '];' + EOL
	x += '} tcli_args_generic_t;' + EOL
	x += '' + EOL
	x += 'typedef union' + EOL
	x += '{' + EOL
	x += '    tcli_args_generic_t generic;' + EOL
	for cmd in cmds:
		x += '    tcli_args' + cmd.structName + '_t ' + cmd.structName[1:] + ';' + EOL
	x += '} tcli_args_t;' + EOL
	x += '' + EOL
	for cmd in cmds:
		x += 'int tcli_cmd_handle' + cmd.structName +  '(tcli_args' + cmd.structName + '_t *args);' + EOL
	x += '' + EOL
	x += '#endif' + EOL
	if debug:
		print(x)
	with open(os.path.join('tcli', 'tcli_def.h'), 'w') as f:
		f.write(x)

	x = ''
	x += '#include "tcli.h"' + EOL
	x += '' + EOL
	x += 'static const char tcli_string_tbl[] =' + EOL
	for word in cmdStringTbl.split(':'):
		if word != '':
			x += '        %-19s // %d' % ('"'+word+'\\000"', cmdStringTbl.find(':'+word+':') + 1) + EOL
	x += '        ;' + EOL + EOL
	x += '#define H(x) ((x) & 0x3fffff)' + EOL +EOL
	x += 'static const tcli_ca_def_t tcli_ca_def[] =' + EOL
	x += '{' + EOL
	for cmd in cmds:
		x += '    //'
		for kw in cmd.keywords:
			x += ' %s' % (kw)
		x += EOL
		idx = [0,0]
		for i in (0,1):
			if len(cmd.keywords) > i:
				idx[i] = cmdStringTbl.find(cmd.keywords[i])
		argCnt = 0
		posCnt = 0
		posReq = 0
		posMulti = 0
		for arg in cmd.args:
			if arg.type.startswith('opt'):
				argCnt += 1
			if arg.type.startswith('pos'):
				posCnt += 1
				if arg.required:
					posReq += 1
			if arg.type == 'posMulti':
				posMulti = 1
		x += '    { .cmd_def.s1_idx = %2d, .cmd_def.s2_idx = %2d, ' % (idx[0], idx[1])
		x += '.cmd_def.arg_def_cnt = %d, ' % (argCnt)
		x += '.cmd_def.pos_cnt = %d, ' % (posCnt)
		x += '.cmd_def.pos_req = %d, ' % (posReq)
		x += '.cmd_def.pos_multi = %d },' % (posMulti)
		x += EOL
		for arg in cmd.args:
			if arg.type.startswith('opt'):
				x += "    { .arg_def.short_char = '%s', " % (' ' if arg.shortName == '' else arg.shortName[1])
				x += '.arg_def.hash = H(0x%x), ' % (hashCalc(arg.longName))
				x += '.arg_def.has_val = %d, ' % (1 if arg.type == 'optVal' else 0)
				x += '.arg_def.mutex = %d, ' % (arg.mutex)
				x += '.arg_def.required = %d }, ' % (arg.required)
				x += '// %s' % (arg.cVarName)
				x += EOL
		x += EOL
	x += '    // end of list' + EOL
	x += '    { .cmd_def.s1_idx =  0, .cmd_def.s2_idx =  0, .cmd_def.arg_def_cnt = 0, .cmd_def.pos_cnt = 0, .cmd_def.pos_req = 0, .cmd_def.pos_multi = 0 },' + EOL
	x += '};' + EOL + EOL
	x += 'const tcli_def_t tcli_def =' + EOL
	x += '{' + EOL
	x += '    .ca_def = &tcli_ca_def[0],' + EOL
	x += '    .cmd_string_tbl = &tcli_string_tbl[0],' + EOL
	x += '};' + EOL
	if debug:
		print(x)
	with open(os.path.join('tcli', 'tcli_def.c'), 'w') as f:
		f.write(x)

	x = ''
	x += '#include "tcli.h"' + EOL + EOL
	x += 'static tcli_args_t args;' + EOL + EOL
	x += 'int tcli_cmd_handle(char *buf)' + EOL
	x += '{' + EOL
	x += '    int rc;' + EOL + EOL
	x += '    memset(&args, 0, sizeof(args));' + EOL
	x += '    if ((rc = tcli_parse(buf, &tcli_def, &args)) < 0) return rc;' + EOL + EOL
	x += '    switch (rc)' + EOL
	x += '    {' + EOL
	for cmd in cmds:
		x += '    %-31s return tcli_cmd_handle%s(&args.%s);' % ('case CMD_ID'+cmd.structName+':', cmd.structName, cmd.structName[1:])
		x += EOL
	x += '    %-31s return TCLI_INTERNAL_ERROR;' % ('default:') + EOL
	x += '    }' + EOL
	x += '}' + EOL + EOL
	for cmd in cmds:
		x += '__attribute__((weak)) int tcli_cmd_handle%s(tcli_args%s_t *args)' % (cmd.structName, cmd.structName)
		x += EOL
		x += '{' + EOL
		x += '    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;' + EOL
		x += '}' + EOL + EOL
	if debug:
		print(x)
	with open(os.path.join('tcli', 'tcli_cmd_handle.c'), 'w') as f:
		f.write(x)	
	
	return 0

if __name__ == '__main__':
    sys.exit(main())
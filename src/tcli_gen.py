import sys
import os

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

def main():
	debug = True
	cmds = []
	EOL = '\n'

	with open('tcli_def.txt', 'r') as f:
		lines = f.readlines()

	for line in lines:
		line = formatLine(line)
		print('Parsing line: "' + line + '"')
		cmd = Cmd()
		parsingKeywords = True
		required = True
		optBoolIdx = 0
		optValIdx = 0
		mutexIdx = 0
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
					required = True
				elif word == ')':
					required = True
				elif word == '[':
					required = False
				elif word == ']':
					required = True
				elif word == '|':
					mutexIdx -= 1
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
						arg.longName = opt
						if sep == '=':
							arg.type = 'optVal'
							arg.optIdx = optValIdx
							optValIdx += 1
						else:
							arg.type = 'optBool'
							arg.optIdx = optBoolIdx
							optBoolIdx += 1
						arg.cVarName = formatCVarName(opt)
					arg.mutexIdx = mutexIdx
					mutexIdx += 1
					mutex = False
					arg.required = required
					cmd.args.append(arg)
		if debug:
			print('Command: ', cmd.keywords)
			for arg in cmd.args:
				print('  Arg: shortName = "' + arg.shortName + '", longName = "' + arg.longName + '", cVarName = "' + arg.cVarName + '", type = ' + arg.type + ', ' + ('required' if arg.required else 'optional') + ', optIdx = ' + str(arg.optIdx) + ', mutexIdx = ' + str(arg.mutexIdx))

		cmds.append(cmd)
	cmdStringTbl = ' :'
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
	print('CmdStringTbl: "' + cmdStringTbl + '"')
	print('ArgStringTbl: "' + argStringTbl + '"')

	tcliDefH  = '#ifndef TCLI_DEF_H' + EOL
	tcliDefH += '#define TCLI_DEF_H' + EOL
	tcliDefH += '' + EOL
	tcliDefH +=	'#include <stdint.h>' + EOL
	tcliDefH +=	'' + EOL
	tcliDefH +=	'enum' + EOL
	tcliDefH +=	'{' + EOL
	tcliDefH +=	'    CMD_ID_NONE = 0,' + EOL
	for cmd in cmds:
		tcliDefH += '    CMD_ID' + cmd.structName + ',' + EOL
	tcliDefH += '    CMD_ID_CNT' + EOL
	tcliDefH += '};' + EOL
	tcliDefH += '' + EOL
	tcliDefH += 'typedef uint32_t bool_options_t;' + EOL
	tcliDefH += '' + EOL
	maxVarArgCnt = 0
	for cmd in cmds:
		varArgCnt = 0
		tcliDefH += 'typedef struct' + EOL
		tcliDefH += '{' + EOL
		boolCnt = 0
		for arg in cmd.args:
			if arg.type == 'optBool':
				tcliDefH += '    bool_options_t ' + arg.cVarName + ':1;' + EOL
				boolCnt += 1
		tcliDefH += '    bool_options_t _pad_:' + str(32-boolCnt) + ';' + EOL		
		for arg in cmd.args:
			if arg.type != 'optBool':
				varArgCnt += 1
				tcliDefH += '    char *' + arg.cVarName + ';' + EOL
		tcliDefH += '} tcli_args' + cmd.structName + '_t;' + EOL
		tcliDefH += '' + EOL
		if varArgCnt > maxVarArgCnt:
			maxVarArgCnt = varArgCnt
	tcliDefH += 'typedef struct' + EOL
	tcliDefH += '{' + EOL
	tcliDefH += '    bool_options_t bools;' + EOL
	tcliDefH += '    char *vals[' + str(maxVarArgCnt) + '];' + EOL
	tcliDefH += '} tcli_args_generic_t;' + EOL
	tcliDefH += '' + EOL
	tcliDefH += 'typedef union' + EOL
	tcliDefH += '{' + EOL
	tcliDefH += '    tcli_args_generic_t generic;' + EOL
	for cmd in cmds:
		tcliDefH += '    tcli_args' + cmd.structName + '_t ' + cmd.structName[1:] + ';' + EOL
	tcliDefH += '} tcli_args_t;' + EOL
	tcliDefH += '' + EOL
	for cmd in cmds:
		tcliDefH += 'int tcli_cmd_handle' + cmd.structName +  '(tcli_args' + cmd.structName + '_t *args);' + EOL
	tcliDefH += '' + EOL
	tcliDefH += '#endif' + EOL

	print(tcliDefH)


	tcliDefC = ''
	tcliDefC += '#include "tcli.h"' + EOL
	tcliDefC += '' + EOL
	tcliDefC += 'const char tcli_string_tbl[] =' + EOL
	for word in cmdStringTbl.split(':'):
		if word != '':
			tcliDefC += '        "' + word + '" // ' + str(cmdStringTbl.find(':'+word+':') + 1) + EOL
	tcliDefC += '' + EOL

	print(tcliDefC)

	with open(os.path.join('tcli', 'tcli_def.h'), 'w') as f:
		f.write(tcliDefH)

	return 0

if __name__ == '__main__':
    sys.exit(main())
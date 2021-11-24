import sys
import os

class Cmd:
	def __init__(self):
		self.keywords = []
		self.args = []

class Arg:
	def __init__(self):
		self.required = False
		self.type = ''
		self.shortName = ''
		self.longName = ''
		self.optIdx = 0
		self.mutexIdx = 0

def formatLine(line):
	line = line.strip()
	line = line.replace('(', ' ( ')
	line = line.replace(')', ' ) ')
	line = line.replace('[', ' [ ')
	line = line.replace(']', ' ] ')
	line = line.replace('|', ' | ')
	return line

def main():
	debug = True

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
						arg.mutexIdx = mutexIdx
						mutexIdx += 1
						mutex = False
					arg.required = required
					cmd.args.append(arg)
		if debug:
			print('Command: ', cmd.keywords)
			for arg in cmd.args:
				print('  Arg: shortName = "' + arg.shortName + '", longName = "' + arg.longName + '", type = ' + arg.type + ', ' + ('required' if arg.required else 'optional') + ', optIdx = ' + str(arg.optIdx) + ', mutexIdx = ' + str(arg.mutexIdx))


if __name__ == '__main__':
    sys.exit(main())
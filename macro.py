import re, sys

class Macro:
	def __init__(self, fp):
		self.filename = fp
		self.variables = {}
		self.macro_ln = re.compile(r'^\s*//\s*MACRO:\s*(.*)')
		self.inline = re.compile(r'{{([^{}]*)}}')

	def parseFile(self):
		self.fp = open(self.filename, 'r')
		self.newFile = open(self.filename + ".otx", 'w')
		for line in self.file:
			macro = self.macro_ln.search(line)
			if macro:
				self.parseMacroLine(macro.group(1))
			else:
				inline_vars = self.inline.findall(line)
				if inline_vars:
					self.parseInline(inline_vars)

		self.fp.close()
		self.newFile.close()


	def parseMacroLine(self, macro):
		parts = macro.split()
		if parts[0] == "DEFINE":
			self.variables[parts[1]] = parts[2]
		elif parts[0] == "LOOP":
			counter = parts[1]
			start = parts[2]
			end = parts[3]

	def parseInline(self, inline_vars):



if __name__ == "__main__":
	m = new Macro(sys.argv[1])
	m.parseFile()
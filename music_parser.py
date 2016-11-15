import traceback

class Parser(object):
    def __init__(self, file):
        self.file = file

    def line(self, line):
        ohead, data = line.split(':', 1)
        data = data.lstrip(' ')
        head = ohead.upper()
        if head == 'INSTRUMENT':
            self.instrument(data)
        elif head == 'SPEED':
            self.tempo(float(data))
        elif head == 'L':
            self.say(data.replace('\\n', '\n')
                         .replace('\\b', '\b')
                         .replace('\\r', '\r'))
        elif head == 'V':
            self.dynamic(data, 0)
        else:
            self.play(ohead, data)

    def instrument(self, new, channel):
        raise NotImplementedError

    def tempo(self, new):
        raise NotImplementedError

    def say(self, new):
        raise NotImplementedError

    def dynamic(self, new, channel):
        raise NotImplementedError

    def play(self, head, data):
        raise NotImplementedError

    def run(self):
        with self.file as file:
            for line in file:
                if not line or line.isspace() or line.startswith((';', '#')):
                    continue
                line = line.rstrip('\r\n')
                if not isinstance(line, unicode):
                    line = line.decode('utf-8', 'ignore')
                try:
                    self.line(line)
                except KeyboardInterrupt:
                    print '^C Interrupted'
                    break
                except Exception:
                    traceback.print_exc()

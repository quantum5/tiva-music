import sys
import time
import traceback

from music_parser import Parser
from ctypes import windll, byref, c_void_p

midiOutOpen = windll.winmm.midiOutOpen
midiOutShortMsg = windll.winmm.midiOutShortMsg
midiOutClose = windll.winmm.midiOutClose
process = windll.kernel32.GetCurrentProcess()

GetPriorityClass = windll.kernel32.GetPriorityClass
SetPriorityClass = windll.kernel32.SetPriorityClass
timeBeginPeriod = windll.winmm.timeBeginPeriod
timeEndPeriod = windll.winmm.timeEndPeriod


class MediaTimer(object):
    def __init__(self, precision=1, priority=0x80):
        self.precision = precision
        self.priority = priority

    def __enter__(self):
        timeBeginPeriod(self.precision)
        self.old_priority = GetPriorityClass(process)
        SetPriorityClass(process, self.priority)

    def __exit__(self, exc_type, exc_value, traceback):
        timeEndPeriod(self.precision)
        SetPriorityClass(process, self.old_priority)


miditable = {'C0': 12, 'C#0': 13, 'Db0': 13, 'D0': 14, 'D#0': 15, 'Eb0': 15, 'E0': 16, 'F0': 17, 'F#0': 18, 'Gb0': 18, 'G0': 19, 'G#0': 20, 'Ab0': 20, 'A0': 21, 'A#0': 22, 'Bb0': 22, 'B0': 23, 'C1': 24, 'C#1': 25, 'Db1': 25, 'D1': 26, 'D#1': 27, 'Eb1': 27, 'E1': 28, 'F1': 29, 'F#1': 30, 'Gb1': 30, 'G1': 31, 'G#1': 32, 'Ab1': 32, 'A1': 33, 'A#1': 34, 'Bb1': 34, 'B1': 35, 'C2': 36, 'C#2': 37, 'Db2': 37, 'D2': 38, 'D#2': 39, 'Eb2': 39, 'E2': 40, 'F2': 41, 'F#2': 42, 'Gb2': 42, 'G2': 43, 'G#2': 44, 'Ab2': 44, 'A2': 45, 'A#2': 46, 'Bb2': 46, 'B2': 47, 'C3': 48, 'C#3': 49, 'Db3': 49, 'D3': 50, 'D#3': 51, 'Eb3': 51, 'E3': 52, 'F3': 53, 'F#3': 54, 'Gb3': 54, 'G3': 55, 'G#3': 56, 'Ab3': 56, 'A3': 57, 'A#3': 58, 'Bb3': 58, 'B3': 59, 'C4': 60, 'C#4': 61, 'Db4': 61, 'D4': 62, 'D#4': 63, 'Eb4': 63, 'E4': 64, 'F4': 65, 'F#4': 66, 'Gb4': 66, 'G4': 67, 'G#4': 68, 'Ab4': 68, 'A4': 69, 'A#4': 70, 'Bb4': 70, 'B4': 71, 'C5': 72, 'C#5': 73, 'Db5': 73, 'D5': 74, 'D#5': 75, 'Eb5': 75, 'E5': 76, 'F5': 77, 'F#5': 78, 'Gb5': 78, 'G5': 79, 'G#5': 80, 'Ab5': 80, 'A5': 81, 'A#5': 82, 'Bb5': 82, 'B5': 83, 'C6': 84, 'C#6': 85, 'Db6': 85, 'D6': 86, 'D#6': 87, 'Eb6': 87, 'E6': 88, 'F6': 89, 'F#6': 90, 'Gb6': 90, 'G6': 91, 'G#6': 92, 'Ab6': 92, 'A6': 93, 'A#6': 94, 'Bb6': 94, 'B6': 95, 'C7': 96, 'C#7': 97, 'Db7': 97, 'D7': 98, 'D#7': 99, 'Eb7': 99, 'E7': 100, 'F7': 101, 'F#7': 102, 'Gb7': 102, 'G7': 103, 'G#7': 104, 'Ab7': 104, 'A7': 105, 'A#7': 106, 'Bb7': 106, 'B7': 107, 'C8': 108, 'C#8': 109, 'Db8': 109, 'D8': 110, 'D#8': 111, 'Eb8': 111}

instruments = {
    'Acoustic Grand Piano': 0,
    'Bright Acoustic Piano': 1,
    'Electric Grand Piano': 2,
    'Honky-tonk Piano': 3,
    'Electric Piano 1': 4,
    'Electric Piano 2': 5,
    'Harpsichord': 6,
    'Clavinet': 7,
    'Celesta': 8,
    'Glockenspiel': 9,
    'Music Box': 10,
    'Vibraphone': 11,
    'Marimba': 12,
    'Xylophone': 13,
    'Tubular Bells': 14,
    'Dulcimer': 15,
    'Drawbar Organ': 16,
    'Percussive Organ': 17,
    'Rock Organ': 18,
    'Church Organ': 19,
    'Reed Organ': 20,
    'Accordion': 21,
    'Harmonica': 22,
    'Tango Accordion': 23,
    'Acoustic Guitar (nylon)': 24,
    'Acoustic Guitar (steel)': 25,
    'Electric Guitar (jazz)': 26,
    'Electric Guitar (clean)': 27,
    'Electric Guitar (muted)': 28,
    'Overdriven Guitar': 29,
    'Distortion Guitar': 30,
    'Guitar harmonics': 31,
    'Acoustic Bass': 32,
    'Electric Bass (finger)': 33,
    'Electric Bass (pick)': 34,
    'Fretless Bass': 35,
    'Slap Bass 1': 36,
    'Slap Bass 2': 37,
    'Synth Bass 1': 38,
    'Synth Bass 2': 39,
    'Violin': 40,
    'Viola': 41,
    'Cello': 42,
    'Contrabass': 43,
    'Tremolo Strings': 44,
    'Pizzicato Strings': 45,
    'Orchestral Harp': 46,
    'Timpani': 47,
    'String Ensemble 1': 48,
    'String Ensemble 2': 49,
    'Synth Strings 1': 50,
    'Synth Strings 2': 51,
    'Choir Aahs': 52,
    'Voice Oohs': 53,
    'Synth Voice': 54,
    'Orchestra Hit': 55,
    'Trumpet': 56,
    'Trombone': 57,
    'Tuba': 58,
    'Muted Trumpet': 59,
    'French Horn': 60,
    'Brass Section': 61,
    'Synth Brass 1': 62,
    'Synth Brass 2': 63,
    'Soprano Sax': 64,
    'Alto Sax': 65,
    'Tenor Sax': 66,
    'Baritone Sax': 67,
    'Oboe': 68,
    'English Horn': 69,
    'Bassoon': 70,
    'Clarinet': 71,
    'Piccolo': 72,
    'Flute': 73,
    'Recorder': 74,
    'Pan Flute': 75,
    'Blown Bottle': 76,
    'Shakuhachi': 77,
    'Whistle': 78,
    'Ocarina': 79,
    'Lead (square)': 80,
    'Lead (sawtooth)': 81,
    'Lead (calliope)': 82,
    'Lead (chiff)': 83,
    'Lead (charang)': 84,
    'Lead (voice)': 85,
    'Lead (fifths)': 86,
    'Lead (bass + lead)': 87,
    'Pad (new age)': 88,
    'Pad (warm)': 89,
    'Pad (polysynth)': 90,
    'Pad (choir)': 91,
    'Pad (bowed)': 92,
    'Pad (metallic)': 93,
    'Pad (halo)': 94,
    'Pad (sweep)': 95,
    'FX (rain)': 96,
    'FX (soundtrack)': 97,
    'FX (crystal)': 98,
    'FX (atmosphere)': 99,
    'FX (brightness)': 100,
    'FX (goblins)': 101,
    'FX (echoes)': 102,
    'FX (sci-fi)': 103,
    'Sitar': 104,
    'Banjo': 105,
    'Shamisen': 106,
    'Koto': 107,
    'Kalimba': 108,
    'Bag pipe': 109,
    'Fiddle': 110,
    'Shanai': 111,
    'Tinkle Bell': 112,
    'Agogo': 113,
    'Steel Drums': 114,
    'Woodblock': 115,
    'Taiko Drum': 116,
    'Melodic Tom': 117,
    'Synth Drum': 118,
    'Reverse Cymbal': 119,
    'Guitar Fret Noise': 120,
    'Breath Noise': 121,
    'Seashore': 122,
    'Bird Tweet': 123,
    'Telephone Ring': 124,
    'Helicopter': 125,
    'Applause': 126,
    'Gunshot': 127,

    'Piano': 0,
    'Phone': 124,
    'Telephone': 124,
    'String Ensemble': 48,
    'Strings': 48,
}

dynamics = {'m': 0x40, 'mf': 0x48, 'f': 0x50, 'ff': 0x60, 'fff': 0x70, 'mp': 0x38, 'p': 0x30, 'pp': 0x28, 'p': 0x20}


class Player(Parser):
    def __init__(self, *args, **kwargs):
        super(Player, self).__init__(*args, **kwargs)
        self.force = 0x40

        self.handle = c_void_p()
        midiOutOpen(byref(self.handle), 0, 0, 0, 0)
        if not self.handle:
            raise RuntimeError("Can't find MIDI device")

    def __del__(self):
        midiOutClose(self.handle)

    def instrument(self, data):
        try:
            id = int(data, 0)
        except ValueError:
            try:
                id = instruments[data]
            except KeyError:
                raise ValueError('Invalid Instrument')
        if 0 <= id < 0x80:
            midiOutShortMsg(self.handle, id << 8 | 0xC0)
        else:
            raise ValueError('Instrument %s out of range' % data)

    def say(self, new):
        self.write(new)

    def dynamic(self, dynamic):
        try:
            force = int(dynamic, 0)
        except ValueError:
            if dynamic in dynamics:
                force = dynamics[dynamic]
            else:
                raise ValueError('Dynamic Invalid: %s' % dynamic)
        if 0 < force < 0x80:
            self.force = force
        else:
            raise ValueError('Dynamic %s out of range' % dynamic)

    def play(self, note, length):
        if note.startswith('0'):
            time.sleep(length/1000.)
            return

        self.write((note, length))

        note = self._getnote(note)
        midiOutShortMsg(self.handle, self.force << 16 | note << 8 | 0x90)
        time.sleep(length / 1000.)
        midiOutShortMsg(self.handle, note << 8 | 0x90)
        if not self.slur:
            time.sleep(0.01)

    def _getnote(self, note):
        try:
            return miditable[note]
        except KeyError:
            raise ValueError("%s doesn't exist in my world" % note)

    def write(self, *args, **kwargs):
        print kwargs.get('sep', '').join(i if isinstance(i, unicode) else unicode(i) for i in args)

    def run(self):
        with MediaTimer():
            super(Player, self).run()

def main():
    import sys
    import codecs
    sys.stdout = codecs.getwriter(sys.stdout.encoding)(sys.stdout, 'backslashreplace')

    for file in sys.argv[1:]:
        try:
            player = Player(open(file))
        except IOError as e:
            print e.message, 'on opening:', file
        else:
            player.run()

if __name__ == '__main__':
    main()

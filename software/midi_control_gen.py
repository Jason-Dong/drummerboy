import mido
from mido import MidiFile
import serial
import glob, os
import random

#opening a port connection to mini-arduino
port = serial.Serial(port="/dev/tty.usbmodem58940301", baudrate=57600)

#Commented Out: generates the sound in the same file
#os.system("drums_rnn_generate --config=drum_kit --bundle_file=drum_kit_rnn.mag --output_dir=generated_midi --num_outputs=5 --num_steps=128 --primer_drums=\"[(36, 42), (), (42,)]\"")

#converts int to the corresponding byte repping kick, snare, tec.
def convert_to_byte_signature(x):
	if x == 36:
		return 1
	elif x == 42:
		return 4
	elif x == 38:
		return 2
	else:
		return 8


temp = []
i = 0

#gets all the midi files generated
midi_test = list(glob.glob("generated_midi/*.mid"))
#number selecting random midi_test
n = random.randint(0, len(midi_test))

#loops drum sample
while True
	#loops through and sends midi signals as bytes real-time
	for message in MidiFile(midi_test[n]).play():
		curr_time = 0
		i+=1
		if i <=2:
			continue
		
		if message.bytes()[2] != 0:
			if message.time == curr_time and message.note in [36,42,38,49]:
				temp.append(convert_to_byte_signature(message.note))
			else:
				print(sum(temp))
				port.write(bytes([sum(temp)]))
				loop_list.append(bytes([sum(temp)]))
				temp = [convert_to_byte_signature(message.note)]

				curr_time += message.time




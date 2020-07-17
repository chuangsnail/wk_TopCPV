#!/usr/bin/env python
import os

def change_info( ChangeNo ):
	os.popen( "FakeData mva 0.22 0 normal {}".format( ChangeNo ) )


def exe():
	import sys

	for i in range(251,500):
		print( "Processing sample {}".format( i ) )
		change_info( i )
		sys.stdout.flush()		#flush the screen

if __name__ == '__main__':
	exe()

#!/usr/bin/env python
import os

def change_info( ChangeNo ):
	os.popen( "det_bias_change_mva 0.22 {}".format( ChangeNo ) )


def exe():
	import sys

	for i in range(1,251):
		print( "Processing sample {}".format( i ) )
		change_info( i )
		sys.stdout.flush()		#flush the screen

if __name__ == '__main__':
	exe()

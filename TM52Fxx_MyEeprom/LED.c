
#include "LED.H"


		unsigned char code Number[]=
		{
		   (sega|segb|segc|segd|sege|segf),                                  // 0
		   (segb|segc),                                                      // 1
		   (sega|segb|segd|sege|segg),                                       // 2
		   (sega|segb|segc|segd|segg),                                       // 3
		   (segb|segc|segf|segg),                                            // 4
		   (sega|segc|segd|segf|segg),                                       // 5
		   (sega|segc|segd|sege|segf|segg),                                  // 6
		   (sega|segb|segc),                                                 // 7
		   (sega|segb|segc|segd|sege|segf|segg),                             // 8
		   (sega|segb|segc|segd|segf|segg),                                  // 9
		   (sega|segd|sege|segf|segg),                                       // E
		   (segg),                                                           // -
		   (sega|segf|sege|segd),											 // C
		   (segb|segc|sege|segf|segg),										 // H
		   (sega|segb|segc|sege|segf|segg),									 //A
		   (segb|segc|segd|sege|segg),                                       //d
		  
		};



volatile unsigned char idata LEDShow[16]= {segg,segg,segg,segg,segg,segg,
								 segg,segg,segg,segg,segg,segg,
								 segg,segg,segg,0};
							 

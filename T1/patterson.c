##########################################################################################################################################################################################
#																				ALGORITIMO DE PATTERSON																					 #
##########################################################################################################################################################################################



flag[0] := false
flag[1] := false
turn := 0
Process P0: Process P1:
 flag[0] := true flag[1] := true
 turn := 1 turn := 0
 while ( flag[1] && turn == 1 ){ while ( flag[0] && turn == 0 ){
 // do nothing // do nothing
 } }
 // critical section // critical section
 ... ...
 // end of critical section // end of critical section
 flag[0] := false flag[1] := false
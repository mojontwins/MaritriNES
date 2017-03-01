' enems_acceleration.bas
' Simulates a 9 to VMAX acceleration

Dim As Integer y, py, pcy, vy, a, vymax, fixBits

y = pcy = 0
vy = 0
a = 1
vymax = 16
fixBits = 4

While vy < vymax
	vy = vy + a
	y = y + vy
	py = y Shr fixBits
	Print (py - pcy) ; ",";
	pcy = py
Wend

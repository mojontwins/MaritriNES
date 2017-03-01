' enemsgravity.bas
' Simulates a gravity fall from vy=0 and generates a LUT of increments to Y

Dim As Integer y, cy, vy, g, vymax, fixBits

y = cy = 0
vy = 0
g = 6
vymax = 96
fixBits = 4

While vy < vymax
	vy = vy + g
	y = y + vy
	Print (y - cy) Shr fixBits; ",";
	cy = y
Wend

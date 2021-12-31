cah = LoadTexture("./resources/cah.png");

function Update() -- DO NOT PUT LOADTEXTURE IN UPDATE LOOP WILL CAUSE MEMORY LEAK
	SetTexture(cah);
	DrawSquare(0, 0, "100%", "100%");
end
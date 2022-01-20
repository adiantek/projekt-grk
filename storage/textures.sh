#!/bin/bash

find assets/textures/ -type f -name "*.png" | sort | while read line
do
	a="${line//\//_}"
	b="${a//assets_textures_/}"
	c="${b//\.png/}"
	echo '    GLuint tex_'"$c"' = 0;'
done

find assets/textures/ -type f -name "*.png" | sort | while read line
do
	a="${line//\//_}"
	b="${a//assets_textures_/}"
	c="${b//\.png/}"
	echo '    loadTexture("'$line'", &this->tex_'"$c"');'
done

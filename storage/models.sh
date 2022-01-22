#!/bin/bash

find assets/models/ -type f | sort | while read line
do
	a="${line//\//_}"
	b="${a//assets_models_/}"
	c="${b//\.png/}"
	d=${cut -d'.'
	echo '    Model *m_'"$c"' = 0;'
done

find assets/textures/ -type f -name "*.png" | sort | while read line
do
	a="${line//\//_}"
	b="${a//assets_textures_/}"
	c="${b//\.png/}"
	echo '    loadTexture("'$line'", &this->tex_'"$c"');'
done

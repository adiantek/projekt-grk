#!/bin/bash

find assets/models/ -type f | sort | while read line
do
	a="${line//\//_}"
	b="${a//assets_models_/}"
	c="${b%.*}"
	echo '    Model *m_'"$c"' = 0;'
done

find assets/models/ -type f | sort | while read line
do
	a="${line//\//_}"
	b="${a//assets_models_/}"
	c="${b%.*}"
	echo '    loadModel("'$line'", &this->m_'"$c"');'
done

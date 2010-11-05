FLOW := $(wildcard *.flow)
PNG := $(FLOW:%.flow=%.png)

all: png

png: statemachine.png $(PNG)
	cat statemachine.dot | sed '$$d' > t.dot
	cat *.flow >> t.dot
	echo >> t.dot
	echo "}" >> t.dot
	dot -Tpng -o full.png t.dot
	mv t.dot full.dot


statemachine.png: statemachine.dot
	dot -Tpng -o statemachine.png statemachine.dot

%.png: %.flow
	cat statemachine.dot | sed '$$d' > t.dot
	cat $< >> t.dot
	echo >> t.dot
	echo "}" >> t.dot
	dot -Tpng -o $<.png t.dot

%.flow: statemachine.dot

clean:
	rm t.dot
	rm *.png


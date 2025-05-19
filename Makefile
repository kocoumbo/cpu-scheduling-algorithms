all: cmpe351

cmpe351:
	gcc cmpe351.c -o cmpe351

clean:
	rm cmpe351

test: cmpe351
	bash test.sh

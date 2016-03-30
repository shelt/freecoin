
#//ONVER:1.0 Must be changed to "client" rule
default:testing

lib:
	cd lib && make

%: lib
	cd $@ && make

all:
	make -f ./Makefile.nm
	make -f ./Makefile.o_tool

# To remove generated files
clean:
	make clean -f ./Makefile.nm
	make clean -f ./Makefile.o_tool

fclean:
	make fclean -f ./Makefile.nm
	make fclean -f ./Makefile.o_tool

re: fclean all

# Detecta se o sistema é de 32 ou 64 bits
BITS_OPTION = -m64

# Define o compilador
CPPC = g++
#############################

# Opções de compilação e includes
CCOPT = $(BITS_OPTION) -O3 -fPIC -fexceptions -DNDEBUG -DIL_STD -std=c++0x
INCLUDEDIR = include
CCFLAGS = $(CCOPT) -I$(INCLUDEDIR)
#############################

# Diretórios com os source files e com os objs files
SRCDIR = src
OBJDIR = obj
#############################

# Lista de todos os srcs e todos os objs
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
#############################

# Regra principal, gera o executável
cvrp: $(OBJS) 
	@echo  "\033[31m \nLinking all objects files: \033[0m"
	$(CPPC) $(BITS_OPTION) $(OBJS) -o $@ $(CCLNFLAGS)
############################

# Inclui os arquivos de dependências
-include $(OBJS:.o=.d)

# Regra para cada arquivo objeto: compila e gera o arquivo de dependências do arquivo objeto
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo  "\033[31m \nCompiling $<: \033[0m"
	$(CPPC) $(CCFLAGS) -c $< -o $@
	@echo  "\033[32m \nCreating $< dependency file: \033[0m"
	$(CPPC) -std=c++0x -MM -MT $@ -I$(INCLUDEDIR) $< > $(basename $@).d
	@mv -f $(basename $@).d $(basename $@).d.tmp
	@sed -e 's|.*:|$(basename $@).o:|' -e 's|$(SRCDIR)/|$(SRCDIR)/|g' < $(basename $@).d.tmp > $(basename $@).d
	@rm -f $(basename $@).d.tmp

# Limpa objetos e arquivos de dependências
clean:
	@echo "\033[31mcleaning obj directory \033[0m"
	@rm cvrp -f $(OBJDIR)/*.o $(OBJDIR)/*.d

# Recompila tudo
rebuild: clean cvrp
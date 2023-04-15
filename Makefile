.PHONY: game all editor start

export NOCLEAR = ""

all: game editor dfm

clearterminal:
	clear
	$(eval export NOCLEAR := "oui")

start: clearterminal
	@mkdir -p release

game: start 
	@printf "\n\033[1;33mBuilding Kuribrawl\n\033[0m"
	$(MAKE) release -C ./c++

editor: start 
	$(MAKE) release -C ./tools/editor/javaEditor
	@mkdir -p release/tools/editor

dfm:
	$(eval NOCLEAR := "true")
	$(MAKE) release -C "./tools/gamefile manager"
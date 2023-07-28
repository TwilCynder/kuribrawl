.PHONY: game all editor start

export NOCLEAR = ""

all: game editor dfm end

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

dfm: start
	$(MAKE) release -C "./tools/gamefile manager"

end: 
	@printf "\033[0;32m\033[1mBuild successful !\n\033[0m"
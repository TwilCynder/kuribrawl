.PHONY: game all editor start

export NOCLEAR = ""
RELEASE=""

all: game editor dfm end

clearterminal:
	clear
	$(eval export NOCLEAR := "oui")

release_:
	@mkdir -p release/tools/editor
	@mkdir -p release/tools/dfm
	@mkdir -p release/game
	$(eval RELEASE := "release")

release: release_ all

start: clearterminal
	@mkdir -p release

game: start 
	@printf "\n\033[1;33mBuilding Kuribrawl\n\033[0m"
	$(MAKE) $(RELEASE) -C ./c++

editor: start 
	@mkdir -p release/tools/editor
	$(MAKE) $(RELEASE) -C ./tools/editor/javaEditor

dfm: start
	$(MAKE) $(RELEASE) -C "./tools/gamefile manager"

data: 
	cd res
	./DFM.exe
	cd ..
	cp ../res/data.twl ../release/kuribrawl/data.twl

end: 
	@printf "\033[0;32m\033[1m======== Build successful ! ========\n\033[0m"
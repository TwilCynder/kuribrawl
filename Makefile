.PHONY: game all editor start

export NOCLEAR = ""
RELEASE=""

all: all_ end
all_: start game editor dfm 

clearterminal:
	clear
	$(eval export NOCLEAR := "oui")

release_:
	@mkdir -p release/tools/editor
	@mkdir -p release/tools/dfm
	@mkdir -p release/game
	$(eval RELEASE := "release")

release: release_ all_ data end

start: clearterminal

game: start 
	@printf "\n\033[1;33mBuilding Kuribrawl\n\033[0m"
	$(MAKE) $(RELEASE) -C ./c++

editor: start 
	@mkdir -p release/tools/editor
	$(MAKE) $(RELEASE) -C ./tools/editor/javaEditor

dfm: start
	$(MAKE) $(RELEASE) -C "./tools/gamefile manager"

data: 
	@printf "\n\033[1;33mBuilding Game Data using the DFM\n\033[0m"
	@(cd res; ./DFM.exe)
	@if [[ ! -z $(RELEASE) ]]; then \
		printf "\n\033[1;33mCopying Game Data to the release folder\n\033[0m"; \
		mkdir -p release/game; \
		cp ./res/data.twl release/game/data.twl; \
	fi
end: 
	@printf "\033[0;32m\033[1m======== Build successful ! ========\n\033[0m"
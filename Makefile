.PHONY: game

all: game editor


clearterminal:
	clear
	$(eval export NOCLEAR := oui)

game: clearterminal
	@printf "\n\033[1;33mBuilding Kuribrawl\n\033[0m"
	$(MAKE) release -C ./c++

editor: clearterminal
	@printf "\n\033[1;33mBuilding the editor\n\033[0m"
	$(MAKE) -C ./tools/editor/javaEditor
	@mkdir -p release/tools/editor
	cp ./tools/editor/javaEditor/kuribrawl-editor.jar ./release/tools/editor
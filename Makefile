.PHONY: game all editor start


all: game editor


clearterminal:
	clear
	$(eval export NOCLEAR := oui)

start: clearterminal
	@mkdir release

game: start 
	@printf "\n\033[1;33mBuilding Kuribrawl\n\033[0m"
	$(MAKE) release -C ./c++

editor: start 
	$(MAKE) -C ./tools/editor/javaEditor
	@mkdir -p release/tools/editor
	cp ./tools/editor/javaEditor/kuribrawl-editor.jar ./release/tools/editor
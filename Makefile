.PHONY: game all editor start


all: game editor

start: 
	@mkdir release

game: start
	$(MAKE) release -C ./c++

editor: start
	$(MAKE) -C ./tools/editor/javaEditor
	@mkdir -p release/tools/editor
	cp ./tools/editor/javaEditor/kuribrawl-editor.jar ./release/tools/editor
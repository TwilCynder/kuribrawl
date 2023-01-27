.PHONY: game

all: game editor

game:
	$(MAKE) release -C ./c++

editor:
	$(MAKE) -C ./tools/editor/javaEditor
	@mkdir -p release/tools/editor
	cp ./tools/editor/javaEditor/kuribrawl-editor.jar ./release/tools/editor
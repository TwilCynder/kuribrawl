.PHONY: game all editor start

export NOCLEAR = ""

all: game editor

start: 
	@mkdir -p release

game: start
	$(eval NOCLEAR := "true")
	$(MAKE) release -C ./c++

editor: start
	$(MAKE) -C ./tools/editor/javaEditor
	@mkdir -p release/tools/editor
	cp ./tools/editor/javaEditor/kuribrawl-editor.jar ./release/tools/editor
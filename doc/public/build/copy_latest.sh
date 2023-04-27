#!/bin/bash
cd "$(dirname "${BASH_SOURCE[0]}")"
cp  "`./latest_version.exe ../../internal/ressource\ file\ format/*`" ..
mv ../0.3.4.md ../resource-format-latest.md
#! /bin/bash
line_regex=^-----\*\$
line_html='<br><div style = "border-top: solid 1px; width: 50\%"><\/div><br>'   

sed "s/$line_regex/$line_html/g" $1
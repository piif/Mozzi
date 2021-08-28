sed -n -e '/{/,/}/ s/^ *//p' $1 \
| sed -e 's/};/]/' -e 's/.*{/[\n/' -e 's/, /\n/g' -e 's/,$//g' \
| grep -v '^$'
> tests/viewWave/inputPipe

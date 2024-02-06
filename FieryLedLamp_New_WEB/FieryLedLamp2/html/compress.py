from jsmin import jsmin

import minify_html

with open('index.html') as file:
    s=file.read()
    print(len(s))
    print(len(minify_html.minify(s)))

with open('index.js') as js_file:
    minified = jsmin(js_file.read())
    print(minified)

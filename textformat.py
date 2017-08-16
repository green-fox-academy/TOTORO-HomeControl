# How to use:
# 1: Exchange required code between 'text' variable's double qoutations
# 2: Open terminal, navigate to textformat.py's directory/folder
# 3: Run it on terminal: 'python textformat.py'
# 4: Et voila - you have the formatted file in 'textformat.txt'

text = "put hex code here"
def insert_newlines(text, every=80):
    lines = []
    for i in xrange(0, len(text), every):
        lines.append(text[i:i+every])
    return '\n'.join(lines)
print("Check \'textformat.txt\' beside of this code.")



f = open('textformat.txt','w')
f.write(insert_newlines(text, 80))
f.close()

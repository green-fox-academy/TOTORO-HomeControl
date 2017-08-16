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

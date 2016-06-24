import sys

if len(sys.argv) != 3:
    print('Error! Wrong arguments given')
    exit(1)

with open(sys.argv[1], 'r') as f:
    with open(sys.argv[2], 'w') as f2:
        lines = f.readlines()
        for i in range(0, len(lines)):
            key = lines[i][0:3] + lines[i][41:46] + '\n'
            f2.write(key)

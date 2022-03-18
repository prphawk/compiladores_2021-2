import sys

out_f = open(sys.argv[1], "r", encoding="utf8")
diag_f = open(sys.argv[2], "w", encoding="utf8")

diag_f.write("digraph G {\n")
dict_nodes = {}
node_count = 0

for line in out_f:
    if "[" not in line:
        nodes = line.replace("\n", "").split(", ")
        for node in nodes:
            if node not in dict_nodes.keys():
                dict_nodes[str(node)] = "n" + str(node_count)
                node_count += 1
            
        new_line = line.replace(", ", " -> ")
        for node in nodes:
            new_line = new_line.replace(node, dict_nodes[str(node)])

        diag_f.write(new_line.replace("\n", "") + ";\n")
    else:
        node = line.split(" ")[0]
        new_line = line.replace(node, dict_nodes[str(node)])
        diag_f.write(new_line)

diag_f.write("\n}")

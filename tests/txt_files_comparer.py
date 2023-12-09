import filecmp

f1 = input("Path to the first txt file:")
f2 = input("Path to the second txt file:")

# Execute comparison.
result = filecmp.cmp(f1, f2, shallow=False)

if not result:
    print(result, "File contents aren't the same. \n")
else:
    print(result, "File contents are the same. \n")
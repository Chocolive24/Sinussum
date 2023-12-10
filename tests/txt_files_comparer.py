import difflib
import os

f1 = ""
# = input("Path to the first txt file:")
f2 = ""
# = input("Path to the second txt file:")

my_files = []
expected_files = []

# assign directory
my_files_dir = os.getcwd()
expected_files_dir = os.path.join(my_files_dir, 'public')

# iterate over files in that directory
for filename in os.listdir(my_files_dir):
    f = os.path.join(my_files_dir, filename)
    # checking if it is a file
    if os.path.isfile(f):
        if filename.startswith('out') and filename.endswith('.txt'):
            my_files.append(f)

for filename in os.listdir(expected_files_dir):
    f = os.path.join(expected_files_dir, filename)
    # checking if it is a file
    if os.path.isfile(f):
        if filename.startswith('out') and filename.endswith('.txt'):
            expected_files.append(f)

for i in range(len(my_files)):
    with open(my_files[i], 'r', newline='') as f1, open(expected_files[i], 'r', newline='') as f2:
        content1 = f1.read().strip()
        content2 = f2.read().strip()

        differ = difflib.Differ()
        diff = list(differ.compare(content1.splitlines(), content2.splitlines()))

        if all(line.startswith(' ') or not line.strip() for line in diff):
            print(f"The files are identical. ", i)
        else:
            print("There is a diff at file number : ", i, "\n")
            for line in diff:
                print(line)
            print("\nContent of the first file:")
            print(content1, "\n")
            print("Content of the second file:")
            print(content2)

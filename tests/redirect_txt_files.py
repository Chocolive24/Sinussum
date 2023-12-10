import os
import subprocess

# Specify the directory containing the text files
directory = 'C:\\Users\\Olivier\\source\\repos\\EPFL\\Sinussum\\tests\\public'

# Specify the path to the executable
executable_path = 'C:\\Users\\Olivier\\source\\repos\\EPFL\\Sinussum\\x64\\Debug\\Sinussum.exe'

# Specify the output directory
output_directory = 'C:\\Users\\Olivier\\source\\repos\\EPFL\\Sinussum\\tests'

# Iterate over files in the directory
for filename in os.listdir(directory):
    file_path = os.path.join(directory, filename)

    # Check if it is a file and has a .txt extension
    if os.path.isfile(file_path) and (filename.startswith('t') and filename.endswith('.txt')):
        # Define the output file path, removing the first two characters
        output_file_path = os.path.join(output_directory, f'out{filename[1:3]}.txt')
        print(f'out{filename[1:3]}.txt')

        # Call the executable and redirect the file to it
        command = [executable_path, '<', file_path, '>', output_file_path]

        try:
            subprocess.run(' '.join(command), shell=True, check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error executing {command}: {e}")


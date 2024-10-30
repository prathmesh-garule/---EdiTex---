import re

def find_and_replace(filename, find_string, replace_string):
    try:
        # Read the file content
        with open(filename, 'r', encoding='utf-8') as file:
            content = file.read()

        # Count occurrences of the find_string (considering multiline)
        occurrences = len(re.findall(re.escape(find_string), content, re.MULTILINE))
        print(f"Occurrences of '{find_string}': {occurrences}")

        # Replace the occurrences
        new_content = re.sub(re.escape(find_string), replace_string, content, flags=re.MULTILINE)

        # Write the updated content back to the file
        with open(filename, 'w', encoding='utf-8') as file:
            file.write(new_content)

        print("Replacement completed.")

    except FileNotFoundError:
        print(f"The file '{filename}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

# Example usage
filename = 'example.txt'            # Replace with your filename
find_string = 'text to find'        # Replace with the string you want to find
replace_string = 'replacement text' # Replace with the replacement string

find_and_replace(filename, find_string, replace_string)
import json

def find_and_replace_in_json(json_file):
    # Hardcoded strings to find and replace
    find_str = "uami"
    replace_str = "new_uami"

    try:
        # Load JSON data from the file
        with open(json_file, 'r') as file:
            data = json.load(file)

        # Convert JSON to a string for find-and-replace, handling newlines
        data_str = json.dumps(data, indent=4)
        
        # Count occurrences of the string to replace
        count = data_str.count(find_str)
        
        # Perform find-and-replace
        new_data_str = data_str.replace(find_str, replace_str)
        
        # Load the modified JSON data back
        new_data = json.loads(new_data_str)
        
        # Save the modified data back to the file
        with open(json_file, 'w') as file:
            json.dump(new_data, file, indent=4)
        
        print(f"Replaced {count} occurrences of '{find_str}' with '{replace_str}' in {json_file}.")

    except FileNotFoundError:
        print(f"Error: File '{json_file}' not found.")
    except json.JSONDecodeError:
        print(f"Error: Failed to parse JSON in '{json_file}'.")
    except Exception as e:
        print(f"An error occurred: {e}")

# Call the function with the JSON file name
json_file = "template.json"  # Replace with your JSON file name
find_and_replace_in_json(json_file)
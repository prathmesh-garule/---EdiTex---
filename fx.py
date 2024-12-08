def add_description(filename, position):
    input_file = path_in + filename + ".csv"

    # Read the file content
    with open(input_file, "r") as f:
        lines = f.readlines()

    # Check if 'description' column already exists
    headers = lines[0].strip().split(",")
    if "description" in headers:
        print("Description column already exists. No changes made.")
        return

    # Open the file to write the updated content
    with open(input_file, "w") as f:
        for i, line in enumerate(lines):
            if i == 0:
                # Insert the 'description' column at the specified position
                headers.insert(position, "description")
                f.write(",".join(headers) + "\n")
            else:
                # Add an empty value for the 'description' column
                row = line.strip().split(",")
                row.insert(position, "")
                f.write(",".join(row) + "\n")

# Example usage
filename = "example"
path_in = "/path/to/csv/"
add_description(filename, 2)
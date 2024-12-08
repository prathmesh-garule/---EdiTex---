def add_column_at_position_five(filename):
    input_file = path_in + filename + ".csv"

    # Read the file content
    with open(input_file, "r") as f:
        lines = f.readlines()

    # Open the file to write the updated content
    with open(input_file, "w") as f:
        for i, line in enumerate(lines):
            # Process the header row
            if i == 0:
                headers = line.strip().split(",")
                # Insert the new column name at the 5th position (index 4)
                headers.insert(4, "description")
                f.write(",".join(headers) + "\n")
            else:
                # For other rows, add an empty value at the 5th position
                row = line.strip().split(",")
                row.insert(4, "")
                f.write(",".join(row) + "\n")

# Example usage
filename = "example"
path_in = "/path/to/csv/"
add_column_at_position_five(filename)
import pandas as pd

def replace_delimiter_clientteam(filename):
    input_path = path_out + filename + ".csv"
    temp_path = path_out + filename + "_temp.csv"

    # Step 1: Replace the delimiter in chunks
    with open(temp_path, "w") as f:
        for chunk in pd.read_csv(input_path, chunksize=50000):
            # Write the chunk to a temporary file with the new delimiter
            chunk.to_csv(f, sep="~", index=False, header=f.tell() == 0, mode='a')

    # Step 2: Replace "~" with "~}|"
    with open(temp_path, "r") as f:
        content = f.read()

    content = content.replace("~", "~}|")

    # Step 3: Write the final content back to the original file
    with open(input_path, "w") as f:
        f.write(content)

    # Step 4: Clean up (optional)
    # If needed, remove the temp file or handle further processing
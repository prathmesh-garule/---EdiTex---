chunk_size = 100000  # Define chunk size
total_rows = df2.count()  # Total rows in the DataFrame

for start in range(0, total_rows, chunk_size):
    chunk_df = df2.filter(f"row_number() BETWEEN {start + 1} AND {start + chunk_size}")  # Fetch rows in chunks
    chunk_df.write.mode("append").parquet("final_filename.parquet")  # Append to Parquet file
    print(f"Processed rows {start + 1} to {start + chunk_size}")
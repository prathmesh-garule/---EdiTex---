chunk_size = 100000  # Define chunk size
total_rows = df2.count()  # Total rows in the DataFrame

for start in range(0, total_rows, chunk_size):
    chunk_df = df2.limit(chunk_size).offset(start)  # Fetch rows in chunks
    pandas_chunk = chunk_df.toPandas()  # Convert the chunk to Pandas
    # Process the pandas_chunk here
    print(f"Processed rows {start} to {start + chunk_size}")
import pandas as pd  # For Pandas DataFrame operations
from pyarrow import parquet  # For Parquet file writing

chunk_size = 100000  # Define chunk size
total_rows = df2.count()  # Total rows in the DataFrame

for start in range(0, total_rows, chunk_size):
    chunk_df = df2.limit(chunk_size).toPandas()  # Fetch rows in chunks and convert to Pandas
    chunk_df.to_parquet("final_filename.parquet", engine="pyarrow", append=True)  # Append to Parquet file
    print(f"Processed rows {start} to {start + chunk_size}")
import pandas as pd
import os

chunk_size = 100000
total_rows = df2.count()
final_filename = "final_filename.parquet"

for start in range(0, total_rows, chunk_size):
    chunk_df = df2.limit(chunk_size).toPandas()
    if os.path.exists(final_filename):
        existing_df = pd.read_parquet(final_filename, engine="pyarrow")
        combined_df = pd.concat([existing_df, chunk_df])
        combined_df.to_parquet(final_filename, engine="pyarrow")
    else:
        chunk_df.to_parquet(final_filename, engine="pyarrow")
    print(f"Processed rows {start} to {start + chunk_size}")

final_df = pd.read_parquet(final_filename, engine="pyarrow")
print(f"Total rows in the final Parquet file: {len(final_df)}")
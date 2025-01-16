chunk_size = 100000  # Define the chunk size
num_chunks = (df2.count() // chunk_size) + 1  # Calculate the number of chunks

chunks = df2.rdd.zipWithIndex().map(lambda x: (x[1] // chunk_size, x[0])).groupByKey()

for chunk_id, chunk_data in chunks.collect():
    chunk_df = spark.createDataFrame(chunk_data)
    pandas_chunk = chunk_df.toPandas()
    # Process or save pandas_chunk here
    print(f"Processed chunk {chunk_id}")
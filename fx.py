from pyspark.sql import SparkSession
from pyspark.sql.functions import col
from pyspark.sql.types import IntegerType, StringType

# Initialize Spark session
spark = SparkSession.builder.appName("ProcessCSV").getOrCreate()

# Define the file paths
input_file = "path/to/input_file.csv"
output_file = "path/to/output_file.csv"

# Define columns to convert to INT
int_columns = ["col1", "col2", "col3", "col4", "col5", "col6"]

# Read the file with the specified delimiter
df = spark.read.option("delimiter", "~}|").option("header", "true").csv(input_file)

# Cast specified columns to INT (if possible), and keep other columns as STRING
for col_name in df.columns:
    if col_name in int_columns:
        # Convert to INT, replacing non-convertible values with NULL
        df = df.withColumn(col_name, col(col_name).cast(IntegerType()))
    else:
        # Ensure other columns are cast as STRING
        df = df.withColumn(col_name, col(col_name).cast(StringType()))

# Write the updated DataFrame back to a CSV file
df.write.option("header", "true").option("delimiter", "~}|").mode("overwrite").csv(output_file)
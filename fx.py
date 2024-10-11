import torch
import torch.nn as nn
import torch.optim as optim
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import OneHotEncoder
from comp_env import environment

# Initialize environment
env = environment()

# Fetch training data
X_train, y_train = env.get_training_data()

# One-hot encode the 'sym' column
encoder = OneHotEncoder(sparse=False)
X_train_sym_encoded = encoder.fit_transform(X_train[['sym']])

# Append encoded 'sym' to the original DataFrame
X_train_encoded = pd.concat([X_train, pd.DataFrame(X_train_sym_encoded, columns=encoder.categories_[0])], axis=1)

# Remove 'sym' column as it is now encoded
X_train_encoded.drop(columns=['sym'], inplace=True)

# Define a neural network model that includes the encoded 'sym'
class VolumePredictor(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(VolumePredictor, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(hidden_size, output_size)

    def forward(self, x):
        out = self.fc1(x)
        out = self.relu(out)
        out = self.fc2(out)
        return out

# Step 1: Prepare the prediction function
def predict(input_df, submission) -> pd.DataFrame:
    # Features used for prediction
    features = ['volumeLastMinute', 'volumeLastHour', 'spread', 'mid'] + list(encoder.categories_[0])

    # One-hot encode 'sym' in the input_df
    input_sym_encoded = encoder.transform(input_df[['sym']])
    
    # Append encoded 'sym' to the input DataFrame
    input_encoded = pd.concat([input_df, pd.DataFrame(input_sym_encoded, columns=encoder.categories_[0])], axis=1)
    
    # Remove the original 'sym' column
    input_encoded.drop(columns=['sym'], inplace=True)
    
    # Convert input_df to tensor
    X_input = torch.tensor(input_encoded[features].values, dtype=torch.float32)

    # Step 2: Initialize the trained model
    input_size = len(features)
    hidden_size = 64
    output_size = 1
    
    model = VolumePredictor(input_size, hidden_size, output_size)
    
    # Step 3: Load pre-trained model if necessary or define training
    # For this example, let's train the model within this function
    # Prepare the data (convert to PyTorch tensors)
    X_train_tensor = torch.tensor(X_train_encoded[features].values, dtype=torch.float32)
    y_train_tensor = torch.tensor(y_train.values, dtype=torch.float32).view(-1, 1)

    # Step 4: Train the model
    criterion = nn.MSELoss()
    optimizer = optim.Adam(model.parameters(), lr=0.001)
    
    num_epochs = 10
    batch_size = 64
    for epoch in range(num_epochs):
        # Forward pass
        outputs = model(X_train_tensor)
        loss = criterion(outputs, y_train_tensor)
        
        # Backward and optimize
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
        
        if (epoch+1) % 2 == 0:
            print(f'Epoch [{epoch+1}/{num_epochs}], Loss: {loss.item():.4f}')
    
    # Step 5: Predict on input data
    with torch.no_grad():
        predicted_volumes = model(X_input).numpy()

    # Step 6: Fill the submission DataFrame
    submission['volumeNextHour'] = predicted_volumes
    
    return submission

# Evaluate the model (Provided by the competition environment)
results = env.evaluate_model(predict, submit=True)
print(results)
import torch
import torch.nn as nn
import torch.optim as optim
import pandas as pd
from sklearn.model_selection import train_test_split
from comp_env import environment

# Initialize environment
env = environment()

# Fetch training data
X_train, y_train = env.get_training_data()

# Define a simple neural network model
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
    features = ['volumeLastMinute', 'volumeLastHour', 'spread', 'mid']

    # Convert input_df to tensor
    X_input = torch.tensor(input_df[features].values, dtype=torch.float32)

    # Step 2: Initialize the trained model
    input_size = len(features)
    hidden_size = 64
    output_size = 1
    
    model = VolumePredictor(input_size, hidden_size, output_size)
    
    # Step 3: Load pre-trained model if necessary or define training
    # For this example, let's train the model within this function
    # Prepare the data (convert to PyTorch tensors)
    X_train_tensor = torch.tensor(X_train[features].values, dtype=torch.float32)
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
import torch
from torch import nn
##from torch.utils.data import Dataset
from torch.utils.data import DataLoader
from torchvision import datasets
from torchvision.transforms import ToTensor
from matplotlib import pyplot as plt


# ***** Parameters *****

# Images are fed to the network in batches of this size and the weights of the
# model are only updated after each batch has been processed
batch_size = 64

# Number of training epochs
n_epochs = 10

# Name of the directory storing the training and testing images
datadir = "Data"

# Name of the figure showing a few example images from the FashionMNIST training dataset
figname = "Some_training_datasets.pdf"

# Name of the file storing the final model parameters
model_filename = "Model.pth"

# **********************


# ---------------------
# Neural newtwork model
# ---------------------
class NeuralNetwork(nn.Module):
    def __init__(self):
        # Call the init method of the parent class (nn.Module)
        super().__init__()

        # Convert multidimensional input to 1D data
        self.flatten = nn.Flatten()

        # Create a stacked sequence of layers
        self.linear_relu_stack = nn.Sequential(
            nn.Linear(28*28, 512),  # Fully connected layer mapping 784 inputs to 512 outputs
            nn.ReLU(),              # Apply the ReLU activation function
            nn.Linear(512, 512),    # Fully connected layer mapping 512 inputs to 512 outputs
            nn.ReLU(),              # Apply the ReLU activation function
            nn.Linear(512, 10)      # Fully connected layer mapping 512 inputs to 10 outputs
        )

    def forward(self, x):
        x      = self.flatten(x)
        logits = self.linear_relu_stack(x)  # Pass the input data through the layers of the network
        return logits  # logit(p) = log(p/(1-p)) for p the classification probability and p/1-p the corresponding odds


# -----------------
# Training function
# -----------------
def train(dataloader, model, f_loss, f_opt):
    size = len(dataloader.dataset)
    model.train()

    for batch, (X, y) in enumerate(dataloader):
        X = X.to(device)
        y = y.to(device)

        # Compute prediction error
        pred = model(X)
        loss = f_loss(pred, y)

        # Backpropagation
        loss.backward()    # Compute the gradient of the loss wrt the model parameters (weights)
        f_opt.step()       # Update the model parameters (weights)
        f_opt.zero_grad()  # Set the gradient accumulators to back to zero

        if batch % 100 == 0:
            loss    = loss.item()
            current = (batch + 1)*len(X)
            print(f"Loss: {loss:>7f}  [{current:>5d}/{size:>5d}]")


# ----------------
# Testing function
# ----------------
def test(dataloader, model, f_loss):
    size        = len(dataloader.dataset)
    num_batches = len(dataloader)

    model.eval()

    test_loss = 0
    correct   = 0

    with torch.no_grad():  # Disable gradients calculation (not needed for testing)
        for X, y in dataloader:  # Loop over dataloader elements, i.e., BATCHES of datasets
            X = X.to(device)
            y = y.to(device)

            pred       = model(X)
            test_loss += f_loss(pred, y).item()  # .item() extracts the scalar value of the loss from the tensor returned by f_loss(pred, y)

            # 'pred' has shape [batch_size, num_classes] and contains the logits,
            # i.e., the unnormalized probabilities that the image X belongs to a
            # certain class y (t-shirt, sneaker, etc.). Then, argmax(1) returns
            # the highest score along the columns, i.e., along the classes.
            correct   += (pred.argmax(1) == y).type(torch.float).sum().item()

    test_loss /= num_batches
    correct   /= size
    print(f"Test Error\n  Accuracy: {(100*correct):>0.1f}%\nAvg loss: {test_loss:>8f}\n")



# ---------------------------------
# Load the data and train the model
# ---------------------------------
# The training and test datasets are only downloaded once
train_dset = datasets.FashionMNIST(root = datadir, train = True,  download = True, transform = ToTensor())
test_dset  = datasets.FashionMNIST(root = datadir, train = False, download = True, transform = ToTensor())

ntrain = len(train_dset)
##ntest  = len(test_dset)


# Visualize a few random items from the training dataset
labels_map = {
    0: "T-Shirt",
    1: "Trouser",
    2: "Pullover",
    3: "Dress",
    4: "Coat",
    5: "Sandal",
    6: "Shirt",
    7: "Sneaker",
    8: "Bag",
    9: "Ankle boot",
}

fig, axes = plt.subplots(3, 3, figsize = (5, 5))

for ax in axes.flat:
    sample_idx = torch.randint(ntrain, size = (1,)).item()
    img, label = train_dset[sample_idx]
    ax.axis("off")
    ax.set_title(labels_map[label])
    ax.imshow(img.squeeze(), cmap = "gray")

fig.tight_layout()
fig.savefig(figname)
plt.close(fig)

print(f"\nExample training images saved to '{figname}'\n")


# Create DataLoader objects wrapping the training and test datasets
train_dl = DataLoader(train_dset, batch_size = batch_size) ##, shuffle = True, num_workers = 4)
test_dl  = DataLoader( test_dset, batch_size = batch_size) ##, shuffle = True, num_workers = 4)

##for X, y in test_dl:  # X is the input, y is the output
####for batch, (X, y) in enumerate(test_dl):  # This would be useful to have the batch size readily available
##    print(type(X), X.shape)
##    print(type(y), y.shape)
##    print("")


# Create a neural network model to classify the images
device = torch.accelerator.current_accelerator().type if torch.accelerator.is_available() else "cpu"
print(f"Device: {device}\n")

model = NeuralNetwork().to(device)  # Do the calculations on the accelerator if available

f_loss = nn.CrossEntropyLoss()
f_opt  = torch.optim.SGD(model.parameters(),  lr = 1.e-03)  # SGD = Stochastic gradient descent, lr = Learning rate
##f_opt  = torch.optim.Adam(model.paramemters(), lr = 1.e-03)

for t in range(n_epochs):
    print(f"Epoch {t+1}\n-------------------------------")
    train(train_dl, model, f_loss, f_opt)
    test(  test_dl, model, f_loss)

print("\nAll done\n")


# Save the model parameters so we don't have to train the network every time
torch.save(model.state_dict(), model_filename)
print(f"Model state saved to '{model_filename}'\n")

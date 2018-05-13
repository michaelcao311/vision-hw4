from uwimg import *

def softmax_model(inputs, outputs):
    l = [make_layer(inputs, outputs, SOFTMAX)]
    return make_model(l)

def neural_net(inputs, outputs):
    print inputs
    l = [   make_layer(inputs, 64, RELU),
            make_layer(64, 32, LRELU),
            make_layer(32, outputs, SOFTMAX)]
    return make_model(l)

print("loading data...")
train = load_classification_data("cifar.train", "cifar/labels.txt", 1)
test  = load_classification_data("cifar.test", "cifar/labels.txt", 1)
print("done")
print

print("training model...")
batch = 128
iters = 3000
rate = 10**(-1)
momentum = .9
decay = 10**(-1)

m = neural_net(train.X.cols, train.y.cols)
train_model(m, train, batch, iters, rate, momentum, decay)
print("done")
print

print("evaluating model...")
print("training accuracy: %f", accuracy_model(m, train))
print("test accuracy:     %f", accuracy_model(m, test))


## Questions ##

# 2.2.1 Why might we be interested in both training accuracy and testing accuracy? What do these two numbers tell us about our current model?
# We are interested in both because we want to know how the model will perform in the 'real world'. The training accuracy tells us how well our model models the training data and testing accuracy tells us how well it performs on data it was not trained on.


# 2.2.2 Try varying the model parameter for learning rate to different powers of 10 (i.e. 10^1, 10^0, 10^-1, 10^-2, 10^-3) and training the model. What patterns do you see and how does the choice of learning rate affect both the loss during training and the final model accuracy?
# 10^-1 was the best performing learning rate for me. Maybe this is because it is fine enough to catch certain trends but not too fine or too coarse.


# 2.2.3 Try varying the parameter for weight decay to different powers of 10: (10^0, 10^-1, 10^-2, 10^-3, 10^-4, 10^-5). How does weight decay affect the final model training and test accuracy?
# Weight decay 10^-2 worked the best for me. The weight decay affects the weight of new data as we progress, so having a low one could cause later data to dominate the weights.


# 2.3.1 Currently the model uses a logistic activation for the first layer. Try using a the different activation functions we programmed. How well do they perform? What's best?
# All the activation functions perform relatively well compared to the linear model. RELU and LRELU performed the best.


# 2.3.2 Using the same activation, find the best (power of 10) learning rate for your model. What is the training accuracy and testing accuracy?
# The best learning rate was 10^-1. The training accuracy was ~0.962 and the testing accuracy was ~0.956



# 2.3.3 Right now the regularization parameter `decay` is set to 0. Try adding some decay to your model. What happens, does it help? Why or why not may this be?
# A little bit of delay helps. This may be because of how the model interacts with the data as it is learning weights.


# 2.3.4 Modify your model so it has 3 layers instead of two. The layers should be `inputs -> 64`, `64 -> 32`, and `32 -> outputs`. Also modify your model to train for 3000 iterations instead of 1000. Look at the training and testing error for different values of decay (powers of 10, 10^-4 -> 10^0). Which is best? Why?
# The smallest decay value worked the best for me. Perhaps this is because we are running many iterations so a smaller decay has time to callibrate the model better.


# 3.2.1 How well does your network perform on the CIFAR dataset?



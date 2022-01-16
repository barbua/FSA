import torch
import numpy as np
import torch.optim as optim

class FSA:
    def __init__(self, k, mu=100, s=0.0001, Niter=300, lr=0.01):
        self.mu = mu
        self.k = k
        self.Niter = Niter
        self.lr = lr
        self.s = s

    def predict(self, X):
        xw = X[:, self.idx].float() @ self.w.view(-1, 1) + self.w0
        return xw

    def fit(self, X, y,device):
        p = X.shape[1]
        y[y == 0] = -1
        self.idx = torch.tensor(np.arange(0, p)).long().to(device)
        self.w = torch.zeros((p, 1))
        self.w0 = torch.zeros(1)
        self.w = self.w.to(device)
        self.w0 = self.w0.to(device)
        self.w0.requires_grad = True
        self.w.requires_grad = True
        optimizer = optim.SGD([self.w, self.w0], lr=self.lr)
        losses = []
        for i in range(self.Niter):
            optimizer.zero_grad()  # zero the gradient buffers
            xw = self.predict(X)
            # print(xw.shape,y.shape)
            yxw = y * xw.squeeze()
            l2 = torch.log(1 + torch.exp(-yxw))
            loss1 = torch.mean(l2[y > 0]) + torch.mean(l2[y < 0]) + self.s * torch.sum(
                self.w ** 2) + self.s * self.w0 ** 2
            # loss1 = torch.mean(l2)+self.s*torch.sum(self.w**2)+self.s*self.w0**2
            loss1.backward()
            optimizer.step()
            m = int(self.k + (p - self.k) * max(0, (self.Niter - 2 * i) / (2 * i * self.mu + self.Niter)))
            if m < self.w.shape[0]:
                sw = -torch.sort(-torch.abs(self.w.view(-1)))[0]
                thr = sw[m - 1].item()
                j = torch.where(torch.abs(self.w.view(-1)) >= thr)[0]
                self.idx = self.idx[j]
                self.w = self.w[torch.abs(self.w) >= thr].detach().clone()
                self.w.requires_grad = True
                optimizer = optim.SGD([self.w, self.w0], lr=0.1)
            # print(i,loss1.item(),m,self.idx.shape)
            losses.append(loss1.item())
        return losses

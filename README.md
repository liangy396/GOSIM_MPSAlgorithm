# GOSIM_MPSAlgorithm
An EM-like optimization algorithm for spatial pattern learning and reproduction for Multiple point statistics simulation

# Paper
Liang Yang, Weisheng Hou, Chanjie Cui, Jie Cui,

GOSIM: A multi-scale iterative multiple-point statistics algorithm with global optimization,

Computers & Geosciences,

Volume 89,

2016,

Pages 57-70,

ISSN 0098-3004,

https://doi.org/10.1016/j.cageo.2015.12.020.

(http://www.sciencedirect.com/science/article/pii/S0098300415301096)

Abstract: 

Most current multiple-point statistics (MPS) algorithms are based on a sequential simulation procedure, during which grid values are updated according to the local data events. Because the realization is updated only once during the sequential process, errors that occur while updating data events cannot be corrected. Error accumulation during simulations decreases the realization quality. Aimed at improving simulation quality, this study presents an MPS algorithm based on global optimization, called GOSIM. An objective function is defined for representing the dissimilarity between a realization and the TI in GOSIM, which is minimized by a multi-scale EM-like iterative method that contains an E-step and M-step in each iteration. The E-step searches for TI patterns that are most similar to the realization and match the conditioning data. A modified PatchMatch algorithm is used to accelerate the search process in E-step. M-step updates the realization based on the most similar patterns found in E-step and matches the global statistics of TI. During categorical data simulation, k-means clustering is used for transforming the obtained continuous realization into a categorical realization. The qualitative and quantitative comparison results of GOSIM, MS-CCSIM and SNESIM suggest that GOSIM has a better pattern reproduction ability for both unconditional and conditional simulations. A sensitivity analysis illustrates that pattern size significantly impacts the time costs and simulation quality. In conditional simulations, the weights of conditioning data should be as small as possible to maintain a good simulation quality. The study shows that big iteration numbers at coarser scales increase simulation quality and small iteration numbers at finer scales significantly save simulation time.

Keywords: MPS algorithm; Global optimization; Iterative algorithm; Geostatistical simulation

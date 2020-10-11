/** About friendly interface
 * Like almost any other choice Friendly_GPIO interface has tradeoffs
 * ^ Enables you to abstract and separate the underlying library from interaction layer
 * so its possible to write code without knowing register level library
 * ⌄ There is some overhead involving the conversion from friendlyHandler structure to
 * Handler used by the modules. Also some extra memory is required. 
 */
function [c_] = clamp(val, min_val, max_val)

c_ = max(min(val, max_val), min_val);
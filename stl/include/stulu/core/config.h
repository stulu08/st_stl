#pragma once
// enable/disable all additional features
#ifndef ST_ADDITIONAL_FEATURES
#define ST_ADDITIONAL_FEATURES 1
#endif // !ST_ADDITIONAL_FEATURES

#ifndef ST_ADDITIONAL_FEATURES_ENABLED_IMPL_FUNC 
#define ST_ADDITIONAL_FEATURES_ENABLED_IMPL_FUNC(x) static ST_INLINE bool AdditionalFeaturesEnable() { return x; }
#endif // !ST_ADDITIONAL_FEATURES_ENABLED_IMPL_FUNC
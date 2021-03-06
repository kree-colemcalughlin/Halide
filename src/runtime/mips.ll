declare <4 x float> @llvm.fabs.v4f32(<4 x float>)

define weak_odr <4 x float> @abs_f32x4(<4 x float> %x) nounwind alwaysinline {
       %tmp = call <4 x float> @llvm.fabs.v4f32(<4 x float> %x)
       ret <4 x float> %tmp
}

declare <2 x float> @llvm.fabs.v2f32(<2 x float>)

define weak_odr <2 x float> @abs_f32x2(<2 x float> %x) nounwind alwaysinline {
       %tmp = call <2 x float> @llvm.fabs.v2f32(<2 x float> %x)
       ret <2 x float> %tmp
}

declare <4 x float> @llvm.sqrt.v4f32(<4 x float>);
declare <2 x double> @llvm.sqrt.v2f64(<2 x double>);

define weak_odr <4 x float> @sqrt_f32x4(<4 x float> %x) nounwind alwaysinline {
       %tmp = call <4 x float> @llvm.sqrt.v4f32(<4 x float> %x)
       ret <4 x float> %tmp
}

define weak_odr <2 x double> @sqrt_f64x2(<2 x double> %x) nounwind alwaysinline {
       %tmp = call <2 x double> @llvm.sqrt.v2f64(<2 x double> %x)
       ret <2 x double> %tmp
}
